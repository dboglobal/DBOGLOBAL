CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2162;
	title = 216202;

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
				conv = 216209;
				ctype = 1;
				idx = 3242401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 216208;
				gtype = 1;
				area = 216201;
				goal = 216204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 216205;
				stype = 2;
				taid = 1;
				title = 216202;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 216208;
			gtype = 1;
			oklnk = 2;
			area = 216201;
			goal = 216204;
			sort = 216205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 216202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 216214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 216201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2611107;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 216207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 51;
			}
			CDboTSCheckClrQst
			{
				and = "2154;";
				flink = 1;
				flinknextqid = "2163;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3242401;";
			}
		}
	}
}

