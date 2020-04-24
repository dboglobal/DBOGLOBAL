CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2312;
	title = 231202;

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
				conv = 231209;
				ctype = 1;
				idx = 3242303;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 231208;
				gtype = 1;
				area = 231201;
				goal = 231204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 231205;
				stype = 1;
				taid = 1;
				title = 231202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 231214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 231201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3131101;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 231207;
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
				and = "2311;";
				flink = 1;
				flinknextqid = "2313;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3242303;";
			}
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
			cont = 231208;
			gtype = 1;
			oklnk = 2;
			area = 231201;
			goal = 231204;
			sort = 231205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 231202;
		}
	}
}

