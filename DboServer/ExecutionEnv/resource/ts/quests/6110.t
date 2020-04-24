CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 1;
	tid = 6110;
	title = 611002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 611007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 7;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "2413102;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 4;
				idx0 = -1;
				rate0 = "1.000000";
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				nidx0 = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 611008;
			gtype = 1;
			oklnk = 2;
			area = 611001;
			goal = 611004;
			sort = 611005;
			prelnk = "0;";
			ds = 0;
			grade = 157103;
			rwd = 100;
			title = 611002;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 611009;
				ctype = 1;
				idx = 2413102;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92037;
				eitype = 4;
				idx0 = 1216;
				rate0 = "1.000000";
				scd0 = 5;
				cnt0 = 30;
				ectype = 0;
				etype = 0;
				nidx0 = -1;
				taid = 3;
				esctype = 1;
			}
			CDboTSActRegQInfo
			{
				cont = 611008;
				gtype = 1;
				area = 611001;
				goal = 611004;
				grade = 157103;
				rwd = 100;
				scitem = -1;
				sort = 611005;
				stype = 4;
				taid = 1;
				title = 611002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 611014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 611001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "2413102;";
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 2;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 378;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
	}
}

