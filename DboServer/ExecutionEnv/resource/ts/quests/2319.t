CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2319;
	title = 231902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 231909;
				ctype = 1;
				idx = 5721301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 231908;
				gtype = 1;
				area = 231901;
				goal = 231904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 231905;
				stype = 1;
				taid = 1;
				title = 231902;
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 231914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 231901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1441101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 231908;
			gtype = 1;
			oklnk = 2;
			area = 231901;
			goal = 231904;
			sort = 231905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 231902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 231907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSCheckClrQst
			{
				and = "2318;";
				flink = 1;
				flinknextqid = "2320;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "5721301;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

