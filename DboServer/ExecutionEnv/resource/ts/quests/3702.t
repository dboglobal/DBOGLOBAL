CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3702;
	title = 370202;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 370207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "3481101;";
			}
			CDboTSCheckClrQst
			{
				and = "3701;";
				flink = 0;
				flinknextqid = "3703;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 370209;
				ctype = 1;
				idx = 3481101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 30;
				eitype = 0;
				idx0 = 17515300;
				cnt0 = 50;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 17515200;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 370208;
				m3widx = 15;
				m1fy = "-116.839996";
				sort = 370205;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-2144.300049";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "-116.839996";
				area = 370201;
				goal = 370204;
				m0fz = "-4258.939941";
				m0widx = 15;
				m1ttip = 370215;
				m2fy = "-114.730003";
				m2pat = 2;
				m3fz = "-4041.250000";
				m3ttip = 370216;
				scitem = -1;
				stype = 2;
				m0fy = "-104.669998";
				m0pat = 2;
				m1fz = "-4041.250000";
				m2fx = "-2419.689941";
				m2ttip = 370216;
				m0fx = "-2272.899902";
				m0ttip = 370215;
				m3pat = 2;
				title = 370202;
				m1fx = "-2144.300049";
				m2fz = "-4272.350098";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 4;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 370208;
			gtype = 3;
			oklnk = 2;
			area = 370201;
			goal = 370204;
			sort = 370205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 370202;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 370214;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 370201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3193102;";
			}
		}
	}
}

