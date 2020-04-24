CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 3;
	rq = 0;
	tid = 822;
	title = 82202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 6411101;
				cnt0 = 4;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "5;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 82208;
			gtype = 2;
			oklnk = 2;
			area = 82201;
			goal = 82204;
			sort = 82205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 82202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 82214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 82201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5062101;";
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
			stdiag = 82207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 28;
			}
			CDboTSCheckClrQst
			{
				and = "821;";
				flink = 1;
				flinknextqid = "2031;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5062101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 82209;
				ctype = 1;
				idx = 5062101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 82208;
				m1fx = "4853.000000";
				m1fy = "0.000000";
				sort = 82205;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "744.000000";
				grade = 132203;
				m0fx = "4853.000000";
				m0ttip = 82215;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 82202;
				gtype = 2;
				area = 82201;
				goal = 82204;
				m0fz = "744.000000";
				m0widx = 1;
				m1ttip = 82216;
				scitem = -1;
				stype = 1;
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
				cnt1 = 0;
				eitype = 0;
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
	}
}

