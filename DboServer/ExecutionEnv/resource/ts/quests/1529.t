CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1529;
	title = 152902;

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
			stdiag = 152907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "3142116;";
			}
			CDboTSCheckClrQst
			{
				and = "1528;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 152909;
				ctype = 1;
				idx = 3142116;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 3502103;
				cnt0 = 1;
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
				cont = 152908;
				area = 152901;
				goal = 152904;
				m0fz = "2222.370117";
				m0widx = 1;
				scitem = -1;
				sort = 152905;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 1;
				grade = 132203;
				m0fx = "-2540.250000";
				m0ttip = 152915;
				rwd = 100;
				taid = 1;
				title = 152902;
				gtype = 3;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 152908;
			gtype = 3;
			oklnk = 2;
			area = 152901;
			goal = 152904;
			sort = 152905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 152902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 152914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 152901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3242201;";
			}
		}
	}
}

