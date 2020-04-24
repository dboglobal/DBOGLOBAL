CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3728;
	title = 372802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372809;
				ctype = 1;
				idx = 3204301;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 12951101;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 12951201;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 372808;
				m3widx = 15;
				m1fy = "-114.089996";
				sort = 372805;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-3721.899902";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "-114.089996";
				area = 372801;
				goal = 372804;
				m0fz = "-5938.000000";
				m0widx = 15;
				m1ttip = 372815;
				m2fy = "-116.519997";
				m2pat = 2;
				m3fz = "-5747.049805";
				m3ttip = 372816;
				scitem = -1;
				stype = 2;
				m0fy = "-116.519997";
				m0pat = 2;
				m1fz = "-5747.049805";
				m2fx = "-3441.929932";
				m2ttip = 372816;
				m0fx = "-3441.929932";
				m0ttip = 372815;
				m3pat = 2;
				title = 372802;
				m1fx = "-3721.899902";
				m2fz = "-5938.000000";
			}
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372814;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3204301;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372807;
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
				npcidx = "3204301;";
			}
			CDboTSCheckClrQst
			{
				and = "3727;";
				flink = 0;
				flinknextqid = "3729;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372808;
			gtype = 2;
			oklnk = 2;
			area = 372801;
			goal = 372804;
			sort = 372805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372802;
		}
	}
}

