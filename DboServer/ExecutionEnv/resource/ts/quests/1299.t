CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 5;
	rq = 0;
	tid = 1299;
	title = 129902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 251;
			elnk = 255;
			nextlnk = 252;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1034;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 3;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "7611203;";
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActETimerE
			{
				taeid = 2;
				taid = 4;
				tceid = 3;
			}
			CDboTSActWorldPlayScript
			{
				sid = 1299;
				start = 0;
				taid = 3;
				uof = 0;
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
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1034;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 129907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 43;
			}
			CDboTSCheckClrQst
			{
				and = "1288;1298;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7611206;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 129908;
			gtype = 1;
			oklnk = 2;
			area = 129901;
			goal = 129904;
			sort = 129905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 129902;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 1034;
			}
			CDboTSClickNPC
			{
				npcidx = "7611203;";
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 251;
			nextlnk = 4;
			prelnk = "7;";

			CDboTSActETimerS
			{
				sort = 0;
				taid = 2;
				tgid = 1;
				time = 300000;
			}
			CDboTSActWorldPlayScript
			{
				sid = 1299;
				start = 1;
				taid = 1;
				uof = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92030;
				cnt1 = 0;
				eitype = 2;
				idx0 = 1034;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 7;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 129909;
				ctype = 1;
				idx = 7611206;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 129908;
				gtype = 1;
				area = 129901;
				goal = 129904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 129905;
				stype = 1;
				taid = 1;
				title = 129902;
			}
		}
		CDboTSContReward
		{
			canclnk = 6;
			cid = 100;
			rwdzeny = 0;
			desc = 129914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 129901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;5;";
			usetbl = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "251;5;2;";
		}
	}
	CNtlTSGroup
	{
		gid = 1;

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
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 129907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 253;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 1034;
				taid = 1;
				type = 1;
			}
		}
	}
}

