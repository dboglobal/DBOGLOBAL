CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3730;
	title = 373002;

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
				conv = 373009;
				ctype = 1;
				idx = 3204101;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 15671100;
				cnt0 = 60;
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
				cont = 373008;
				m1fx = "-3428.489990";
				m2fz = "-5468.839844";
				m1fy = "-116.370003";
				sort = 373005;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 373001;
				goal = 373004;
				m0fz = "-5331.060059";
				m0widx = 15;
				m1ttip = 373015;
				m2fy = "-111.419998";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "-105.650002";
				m0pat = 2;
				m1fz = "-5447.930176";
				m2fx = "-3229.090088";
				m2ttip = 373015;
				m0fx = "-3566.360107";
				m0ttip = 373015;
				title = 373002;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 373008;
			gtype = 3;
			oklnk = 2;
			area = 373001;
			goal = 373004;
			sort = 373005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 373002;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 373007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSCheckClrQst
			{
				and = "3729;";
				flink = 0;
				flinknextqid = "3731;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3204101;";
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 373014;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 373001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3204101;";
			}
		}
	}
}

