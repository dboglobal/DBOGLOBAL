CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 484;
	title = 48402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "4372104;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 101;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1121;
			}
			CDboTSClickNPC
			{
				npcidx = "4372104;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActETimerE
			{
				taeid = 4;
				taid = 3;
				tceid = 2;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1121;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 2;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 4;
				tgid = 1;
				time = 480000;
			}
			CDboTSActNPCConv
			{
				conv = 48409;
				ctype = 1;
				idx = 4111201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 48408;
				gtype = 1;
				area = 48401;
				goal = 48404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 48405;
				stype = 1;
				taid = 1;
				title = 48402;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92010;
				cnt1 = 0;
				eitype = 2;
				idx0 = 1121;
				cnt0 = 1;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 3;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 48407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 13;
			}
			CDboTSClickNPC
			{
				npcidx = "4111201;";
			}
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 48414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 48401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;101;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 48408;
			gtype = 1;
			oklnk = 2;
			area = 48401;
			goal = 48404;
			sort = 48405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 48402;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 48407;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1121;
				taid = 1;
				type = 1;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 1;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActTSState
			{
				state = 2;
				taid = 1;
				type = 0;
			}
		}
	}
}

