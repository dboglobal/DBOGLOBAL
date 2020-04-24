CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1782;
	title = 178202;

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
				conv = 178209;
				ctype = 1;
				idx = 5411102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 178208;
				gtype = 1;
				area = 178201;
				goal = 178204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 178205;
				stype = 2;
				taid = 1;
				title = 178202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 178207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSClickNPC
			{
				npcidx = "5411102;";
			}
			CDboTSCheckClrQst
			{
				and = "1726;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
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
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 178208;
			gtype = 1;
			oklnk = 2;
			area = 178201;
			goal = 178204;
			sort = 178205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 178202;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 178214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 178201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111102;";
			}
		}
	}
}

