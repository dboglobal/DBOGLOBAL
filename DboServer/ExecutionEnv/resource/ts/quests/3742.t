CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 3742;
	title = 374202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 374207;
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
				npcidx = "2532149;";
			}
			CDboTSCheckClrQst
			{
				and = "3741;";
				flink = 0;
				flinknextqid = "3743;";
				not = 0;
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
				conv = 374209;
				ctype = 1;
				idx = 2532149;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 11521100;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 16621100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 374208;
				m3widx = 15;
				m1fy = "0.000000";
				sort = 374205;
				m2widx = 15;
				grade = 132203;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-5134.315918";
				rwd = 100;
				taid = 1;
				gtype = 3;
				m3fy = "0.000000";
				area = 374201;
				goal = 374204;
				m0fz = "-4024.520020";
				m0widx = 15;
				m1ttip = 374216;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-3890.882080";
				m3ttip = 374216;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3964.780029";
				m2fx = "-5134.315918";
				m2ttip = 374215;
				m0fx = "-5234.009766";
				m0ttip = 374215;
				m3pat = 2;
				title = 374202;
				m1fx = "-5279.250000";
				m2fz = "-3890.882080";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 374214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 374201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2532149;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 374208;
			gtype = 3;
			oklnk = 2;
			area = 374201;
			goal = 374204;
			sort = 374205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 374202;
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

