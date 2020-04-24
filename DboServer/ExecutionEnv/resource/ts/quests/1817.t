CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1817;
	title = 181702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 181707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 33;
			}
			CDboTSClickNPC
			{
				npcidx = "4072104;";
			}
			CDboTSCheckClrQst
			{
				and = "948;";
				flink = 1;
				not = 0;
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
				itype = 0;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 181714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 181701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4072104;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 181708;
			gtype = 2;
			oklnk = 2;
			area = 181701;
			goal = 181704;
			sort = 181705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 181702;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 181709;
				ctype = 1;
				idx = 4072104;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8441100;
				cnt0 = 11;
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
				cont = 181708;
				m1fx = "4065.840088";
				m1fy = "0.000000";
				sort = 181705;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "405.000000";
				grade = 132203;
				m0fx = "3441.629883";
				m0ttip = 181715;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 181702;
				gtype = 2;
				area = 181701;
				goal = 181704;
				m0fz = "-14.920000";
				m0widx = 1;
				m1ttip = 181715;
				scitem = -1;
				stype = 1;
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
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;0;1;";
			type = 0;
		}
	}
}

