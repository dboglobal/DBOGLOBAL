CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3017;
	title = 301702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 301707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSCheckClrQst
			{
				and = "3016;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4751601;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 301709;
				ctype = 1;
				idx = 4751601;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99156;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 301708;
				m1fx = "4650.979980";
				m1fy = "0.000000";
				sort = 301705;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-5021.029785";
				grade = -1;
				m0fx = "4550.279785";
				m0ttip = 301715;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 301702;
				gtype = 1;
				area = 301701;
				goal = 301704;
				m0fz = "-4932.279785";
				m0widx = 1;
				m1ttip = 301715;
				scitem = 99156;
				stype = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 3017;
				cnt0 = 4;
				ectype = -1;
				etype = 0;
				qtidx = 301715;
				taid = 3;
				esctype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 301714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 301701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751601;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActItem
			{
				iidx0 = 99156;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 301708;
			gtype = 1;
			oklnk = 2;
			area = 301701;
			goal = 301704;
			sort = 301705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 301702;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

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

			CDboTSActItem
			{
				iidx0 = 99156;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
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
}

