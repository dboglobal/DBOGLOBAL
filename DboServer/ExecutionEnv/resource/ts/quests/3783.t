CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3783;
	title = 378302;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 378307;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 69;
			}
			CDboTSClickNPC
			{
				npcidx = "3333201;";
			}
			CDboTSCheckClrQst
			{
				and = "3781;";
				flink = 1;
				flinknextqid = "3784;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 378320;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 6;

			CDboTSColRgn
			{
				widx = 15;
				cct = 1;
				crt = 0;
				r = "20.000000";
				x = "-4569.509766";
				z = "-5382.290039";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 378309;
				ctype = 1;
				idx = 3333201;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 378308;
				area = 378301;
				goal = 378304;
				m0fz = "-5382.290039";
				m0widx = 15;
				scitem = -1;
				sort = 378305;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "-4569.509766";
				m0ttip = 378315;
				rwd = 100;
				taid = 1;
				title = 378302;
				gtype = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 378314;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 378301;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3333201;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 378308;
			gtype = 1;
			oklnk = 2;
			area = 378301;
			goal = 378304;
			sort = 378305;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 378302;
		}
	}
}

