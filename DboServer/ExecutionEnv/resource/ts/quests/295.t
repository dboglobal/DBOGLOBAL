CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 295;
	title = 29502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 2;
			prelnk = "4;";

			CDboTSActNPCConv
			{
				conv = 29502;
				ctype = 1;
				idx = 4752101;
				taid = 1;
			}
			CDboTSActItem
			{
				iidx0 = 99026;
				stype0 = 1;
				taid = 2;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "1;";
			nolnk = 5;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99026;
			}
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
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 29507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 14;
			}
			CDboTSClickNPC
			{
				npcidx = "4752101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "5;4;";

			CDboTSActNPCConv
			{
				conv = 29509;
				ctype = 1;
				idx = 4752101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
				idx0 = 486;
				cnt0 = 5;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 29508;
				m1fx = "4886.450195";
				m1fy = "0.000000";
				sort = 29505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2398.929932";
				grade = 132203;
				m0fx = "5080.020020";
				m0ttip = 29515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 29502;
				gtype = 2;
				area = 29501;
				goal = 29504;
				m0fz = "-2316.189941";
				m0widx = 1;
				m1ttip = 29515;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 29514;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 29501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSCheckSToCEvt
			{
				itype = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4752101;";
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActItem
			{
				iidx0 = 99026;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 3;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 29508;
			gtype = 2;
			oklnk = 4;
			area = 29501;
			goal = 29504;
			sort = 29505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 29502;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99026;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 486;
				taid = 1;
				type = 1;
			}
		}
	}
}

