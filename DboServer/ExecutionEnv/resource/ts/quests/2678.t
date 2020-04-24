CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2678;
	title = 267802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 5;
			prelnk = "100;4;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "2793101;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 267820;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 1;
				cct = 1;
				crt = 0;
				r = "15.000000";
				x = "479.000000";
				z = "-4127.000000";
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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 267807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "2793101;";
			}
			CDboTSCheckClrQst
			{
				and = "2677;";
				flink = 1;
				flinknextqid = "2679;";
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
				conv = 267809;
				ctype = 1;
				idx = 2793101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 267808;
				area = 267801;
				goal = 267804;
				m0fz = "-4097.000000";
				m0widx = 1;
				scitem = -1;
				sort = 267805;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "391.000000";
				m0ttip = 267815;
				rwd = 100;
				taid = 1;
				title = 267802;
				gtype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 267814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 267801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 267808;
			gtype = 2;
			oklnk = 2;
			area = 267801;
			goal = 267804;
			sort = 267805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 267802;
		}
	}
}

