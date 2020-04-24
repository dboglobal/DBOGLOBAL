CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1380;
	title = 138002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
			CDboTSActItem
			{
				iidx0 = 99036;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActItem
			{
				iidx0 = 99036;
				stype0 = 1;
				taid = 1;
				type = 0;
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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 138009;
				ctype = 1;
				idx = 7131122;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 182;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = -1;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 138008;
				area = 138001;
				goal = 138004;
				m0fz = "4737.000000";
				m0widx = 1;
				scitem = -1;
				sort = 138005;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2581.000000";
				m0ttip = 138015;
				rwd = 100;
				taid = 1;
				title = 138002;
				gtype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 138008;
			gtype = 2;
			oklnk = 2;
			area = 138001;
			goal = 138004;
			sort = 138005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 138002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 138014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 138001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7131122;";
			}
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
			stdiag = 138007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 44;
			}
			CDboTSCheckClrQst
			{
				and = "1379;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "7131122;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx1 = 99037;
				stype0 = 1;
				iidx0 = 99036;
				stype1 = 1;
				taid = 255;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 138007;
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
	}
}

